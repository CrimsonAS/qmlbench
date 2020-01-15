/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the qmlbench tool.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QSet>
#include <QVector>

#include <algorithm>

static QJsonObject loadFile(const QString &fileName, QString *error)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        *error = f.errorString();
        return QJsonObject();
    }
    QJsonParseError jsonError;
    auto document = QJsonDocument::fromJson(f.readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        *error = jsonError.errorString();
        return QJsonObject();
    }

    if (!document.isObject()) {
        *error = QLatin1String("JSON data is an array, expected an object");
        return QJsonObject();
    }

    return document.object();
}

struct Result
{
    Result() {}
    Result(const QString &name, double differenceInPercent)
        : name(name), differenceInPercent(differenceInPercent)
    {}
    QString name;
    double differenceInPercent = 0;
};

static QString trimPrefix(const QString &str, const QString &prefix)
{
    if (!str.startsWith(prefix))
        return str;
    QString result(str);
    result.remove(0, prefix.length());
    return result;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Compare results of json output of qmlbench");
    parser.addHelpOption();

    QCommandLineOption keyOption("key");
    keyOption.setDescription(QCoreApplication::translate("main", "Result key in JSON data to compare"));
    keyOption.setDefaultValue("average");
    parser.addOption(keyOption);

    QCommandLineOption prefixOption("prefix");
    prefixOption.setDescription(QCoreApplication::translate("main", "Prefix to strip from test case names"));
    prefixOption.setValueName(QString("path"));
    parser.addOption(prefixOption);

    parser.addPositionalArgument("baseline", QCoreApplication::translate("main", "Base line or reference performance results, in JSON format"));
    parser.addPositionalArgument("results", QCoreApplication::translate("main", "New results to compare against the base line, in JSON format"));

    parser.process(app);

    const QString key = parser.value(keyOption);
    const QString prefix = parser.value(prefixOption);

    QStringList args = parser.positionalArguments();
    if (args.count() != 2) {
        qFatal("Internal error, two arguments are required, but QCommandLineParser should have checked for that...");
        return EXIT_FAILURE;
    }

    const QString baseLineFileName = args.at(0);
    const QString newResultsFileName = args.at(1);

    QString error;
    QJsonObject baseLine = loadFile(baseLineFileName, &error);
    if (!error.isEmpty()) {
        qWarning("Error loading reference file %s: %s", qPrintable(baseLineFileName), qPrintable(error));
        return EXIT_FAILURE;
    }

    QJsonObject newResults = loadFile(newResultsFileName, &error);
    if (!error.isEmpty()) {
        qWarning("Error loading new results file %s: %s", qPrintable(baseLineFileName), qPrintable(error));
        return EXIT_FAILURE;
    }

    const QSet<QString> unrelatedKeys{"command-line", "id", "opengl", "os", "qt", "windowSize"};

    auto filter = [&unrelatedKeys](const QStringList &list) -> QStringList {
        return QSet<QString>(list.cbegin(), list.cend()).subtract(unrelatedKeys).values();
    };

    QStringList tests = filter(baseLine.keys());
    QStringList newResultKeys = filter(newResults.keys());
    if (tests != newResultKeys) {
        QSet<QString> baseLineSet(tests.cbegin(), tests.cend());
        QSet<QString> resultsSet(newResultKeys.cbegin(), newResultKeys.cend());

        qWarning("Error: The two result files do not cover the same set of tests");
        qWarning("Tests existing in the base line but missing from the new results: %s", qPrintable(baseLineSet.subtract(resultsSet).values().join("\t\n")));
        qWarning("Tests existing in the new results but missing from the base line: %s", qPrintable(resultsSet.subtract(baseLineSet).values().join("\t\n")));

        return EXIT_FAILURE;
    }

    QVector<Result> differencesInPercent;

    for (const QString &test: qAsConst(tests)) {
        const QJsonObject baseLineResult = baseLine[test].toObject();
        const QJsonObject newResult = newResults[test].toObject();
        const QString testName = trimPrefix(test, prefix);

        const double oldValue = baseLineResult[key].toDouble();
        const double newValue = newResult[key].toDouble();

        const double differenceInPercent = (newValue - oldValue) * 100 / oldValue;

        if (differenceInPercent > 0) {
            printf("%s: improvement by %.2f%%\n", qPrintable(testName), differenceInPercent);
        } else if (differenceInPercent < 0) {
            printf("%s: regression by %.2f%%\n", qPrintable(testName), differenceInPercent);
        }

        differencesInPercent << Result(testName, differenceInPercent);
    }

    auto minMax = std::minmax_element(differencesInPercent.constBegin(), differencesInPercent.constEnd(), [](const Result &lhs, const Result &rhs) {
        return lhs.differenceInPercent < rhs.differenceInPercent;
    });

    const Result biggestLoser = *minMax.first;
    const Result biggestWinner = *minMax.second;

    printf("Biggest improvement: %s with %.2f%%\n", qPrintable(biggestWinner.name), biggestWinner.differenceInPercent);
    printf("Biggest regression: %s with %.2f%%\n", qPrintable(biggestLoser.name), biggestLoser.differenceInPercent);

    double overallAverage = std::accumulate(differencesInPercent.constBegin(), differencesInPercent.constEnd(), 0.0, [](double v, const Result &r) {
        return v + r.differenceInPercent; })
        / double(differencesInPercent.count());
    printf("Overall average of differences: %.2f%%\n", overallAverage);

    return EXIT_SUCCESS;
}
