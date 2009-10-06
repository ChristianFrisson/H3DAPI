from H3DInterface import *

nodes = references.getValue()

print nodes

gfxSlats = nodes[0:5]
trnsSlats = nodes[5:10]

print gfxSlats
print trnsSlats

#  dimension of the main object
dx, dy, dz = [0.4, 0.05, 0.2]

# height and width of the 4 extra edges
eh, ew = [0.02, 0.01]

# gfxSlat 0 1 2 3 4: main up right down left
gfxSlats[0].size.setValue(Vec3f(dx, dy, dz))

gfxSlats[1].size.setValue(Vec3f(dx, eh, ew))
gfxSlats[3].size.setValue(Vec3f(dx, eh, ew))

gfxSlats[2].size.setValue(Vec3f(ew, eh, dz))
gfxSlats[4].size.setValue(Vec3f(ew, eh, dz))

trnsSlats[1].translation.setValue(Vec3f(0, (dy-eh)/2+eh, (dz-ew)/2))
trnsSlats[3].translation.setValue(Vec3f(0, (dy-eh)/2+eh, -(dz-ew)/2))
trnsSlats[2].translation.setValue(Vec3f((dx-ew)/2, (dy-eh)/2+eh, 0))
trnsSlats[4].translation.setValue(Vec3f(-(dx-ew)/2, (dy-eh)/2+eh, 0))