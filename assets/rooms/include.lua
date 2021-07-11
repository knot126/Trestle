-- 
-- Copyright (C) 2021 Decent Games
-- -------------------------------
--
-- Include script for levels
-- 

QR_COMPONENT_TRANSFORM = (1 << 0)
QR_COMPONENT_MESH = (1 << 1)
QR_COMPONENT_PHYSICS = (1 << 3)

function loadModel(x, y, z, sx, sy, sz, model)
	local flags = QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH
	local ent = mgEntity(flags)
	
	mgTransform(ent, x, y, z, 0.0, 0.0, 0.0, sx, sy, sz)
	mgObjMesh(ent, model)
	
	return ent
end
