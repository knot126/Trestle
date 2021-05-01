	uint32_t ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH);
	entity_load_mesh(&main_world, ent, "assets://mesh/cube2.bin");
	entity_set_transform(&main_world, ent, DgVec3New(0.0f, 0.0f, 0.2f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(3.0f, 0.1f, 8.0f));
	
	ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_MESH | QR_COMPONENT_PHYSICS);
	entity_set_transform(&main_world, ent, DgVec3New(-1.25f, 0.0f, 0.0f), DgVec3New(0.0f, 0.0f, 0.0f), DgVec3New(1.0f, 1.5f, 1.0f));
	entity_phys_set_flags(&main_world, ent, 0);
	entity_phys_set_mass(&main_world, ent, 100.0f);
	entity_phys_add_force(&main_world, ent, DgVec3New(0.0f, 320.0f, -120.0f), DgVec3New(0.0f, 0.0f, -0.1f));
	entity_load_mesh(&main_world, ent, "assets://mesh/cube3.bin");
	
	uint32_t ent = world_create_entity(&main_world, QR_COMPONENT_TRANSFORM | QR_COMPONENT_CAMERA | QR_COMPONENT_PHYSICS);
	entity_set_transform(&main_world, ent, DgVec3New(0.0f, 2.0f, 3.0f), DgVec3New(0.1f, 0.0f, 0.0f), DgVec3New(1.0f, 1.0f, 1.0f));
	entity_phys_set_flags(&main_world, ent, QR_PHYS_DISABLE_GRAVITY);
	entity_phys_set_mass(&main_world, ent, 10.0f);
	entity_phys_add_force(&main_world, ent, DgVec3New(0.0f, 0.0f, -0.25f), DgVec3New(0.0f, 0.0f, 0.0f));
	world_set_camera(&main_world, ent);
