# creation

This is a suite of tests to find out how good we are with creation, rendering
and destruction of objects. Some of the tests are also written in such a way
that they can be compared between each other -- these are usually noted in the
descriptive comment at the top of the test.

Creation is an important factor: our items should be light, as creating a dialog
or page of UI can often creating a few hundred different items (especially Text,
Item, Rectangle, etc). In addition to this, there's code outside of our control
on the application end: JS logic, model interaction, database or file I/O all
comes in addition - so we need to leave plenty of performance left over for the
end user.

