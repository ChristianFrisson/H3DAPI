from H3DInterface import *


shader_node = getNamedNode( "SHADER" )
scene = getCurrentScenes()[0]
window = scene.window.getValue()[0]

window.width.route(shader_node.width)
window.height.route(shader_node.height)

