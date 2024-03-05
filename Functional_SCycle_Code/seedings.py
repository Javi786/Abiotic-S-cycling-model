from generate_configs import generate_reservoirs, generate_parameters
r1 = {
   'name': 'Atmosphere',
   'element': 's_ox',
   'log': True,
   'vmin': 1e5,
   'vmax': 1e16
 }
r2 = {
   'name': 'Oceans',
   'element': 's_ox',
   'log': True,
   'vmin': 1e12,
   'vmax': 1e19
 }
r2_1 = {
   'name': 'Oceans',
   'element': 's_red',
   'log': True,
   'vmin': 1e12,
   'vmax': 1e19
 }
 
#Specify 7 reservoirs for random initial conditions.
r3 = {
   'name': 'Sediments',
   'element': 's_ox',
   'log': True,
   'vmin': 1,
   'vmax': 1e2
}
r3_1 = {
   'name': 'Sediments',
   'element': 's_red',
   'log': True,
   'vmin': 1,
   'vmax': 1e2
}
r4 = {
   'name': 'OCrust',
   'element': 's_ox',
   'log': True,
   'vmin': 1e10,
   'vmax': 2.08e21
}
r4_1 = {
   'name': 'OCrust',
   'element': 's_red',
   'log': True,
   'vmin': 1e10,
   'vmax': 2.08e21
}
r5 = {
   'name': 'CCrust',
   'element': 's_ox',
   'log': True,
   'vmin': 1,
   'vmax': 1e2
}
r5_1 = {
   'name': 'CCrust',
   'element': 's_red',
   'log': True,
   'vmin': 1,
   'vmax': 1e2
}
r6 = {
   'name': 'UMantle',
   'element': 's_red',
   'log': True,
   'vmin': 1e18,
   'vmax': 2.08e21
}
r7 = {
   'name': 'LMantle',
   'element': 's_red',
   'log': True,
   'vmin': 1e18,
   'vmax': 2.08e21
}
generate_reservoirs([r1, r2, r2_1, r3, r3_1, r4, r4_1, r5, r5_1, r6, r7], m=2.08e21, n=30, default="./new_config.yaml")
