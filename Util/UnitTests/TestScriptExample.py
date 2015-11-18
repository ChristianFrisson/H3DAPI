"""
An example test script
Any number of functions can be defined here. The test functions shouldn't take any parameters.
Once they're all defined you simply call testHelper.addTests() and pass it a list of the functions to run.
If the test is a screenshot test then a screenshot will be taken immediately after the x3d file is done loading.
Then every test function you pass to testHelper.addTests will be run.
You can also call testHelper.takeScreenshot(), which takes an optional string parameter for the name of the test (if none is provided than an autoincrementing number will be suffixed to the file name) and takes a screenshot.
H3D will run for at least a second between every call to one your specified test functions.
"""


def testRotation():
  trans = getNamedNode('T')
  rot = trans.getField('rotation')
  rot.setValue(Rotation(50, 0, 20, 1))

testHelper.addTests([testRotation])
