from H3DInterface import *
from H3DUtils import *



scene = getCurrentScenes()[0]
window = scene.window.getValue()[0]

shader_clearBuffer = getNamedNode("shader_clearBuffer")

shader_renderContent = getNamedNode("shader_renderContent")
shader_renderContent2 = getNamedNode("shader_renderContent2")

shader_resolve = getNamedNode("shader_resolve")


if (shader_renderContent):
    window.width.route(shader_renderContent.SCREEN_WIDTH)

    window.height.route(shader_renderContent.SCREEN_HEIGHT)

if (shader_renderContent2):
    window.width.route(shader_renderContent2.SCREEN_WIDTH)

    window.height.route(shader_renderContent2.SCREEN_HEIGHT)

window.width.route(shader_clearBuffer.SCREEN_WIDTH)
window.height.route(shader_clearBuffer.SCREEN_HEIGHT)

window.width.route(shader_resolve.SCREEN_WIDTH)
window.height.route(shader_resolve.SCREEN_HEIGHT)

abufferImg = getNamedNode("abufferImg")
abufferCounterImg = getNamedNode("abufferCounterImg")

window.width.route(abufferImg.width)
window.height.route(abufferImg.height)

window.width.route(abufferCounterImg.width)
window.height.route(abufferCounterImg.height)

fb_CLEARABUFFER = getNamedNode("fb_CLEARABUFFER")
fb_renderContent = getNamedNode("fb_renderContent")
fb_resolve = getNamedNode("fb_resolve")

window.width.route(fb_CLEARABUFFER.width)
window.height.route(fb_CLEARABUFFER.height)

window.width.route(fb_renderContent.width)
window.height.route(fb_renderContent.height)



