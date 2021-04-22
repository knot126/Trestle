-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Game Startup Script
-- 

QR_COMPONENT_TRANSFORM = (1 << 0)
QR_COMPONENT_MESH = (1 << 1)
QR_COMPONENT_CAMERA = (1 << 2)
QR_COMPONENT_PHYSICS = (1 << 3)

ent = mgCreateEntity(QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH)

print("Made entity from lua; Id: ")
print(ent)

mgSetTransform(ent, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0)
mgSetMesh(ent, "assets://mesh/cube2.bin")
