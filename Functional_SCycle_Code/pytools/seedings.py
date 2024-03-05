from generate_configs import generate_reservoirs, generate_parameters
r1 = {
   'name': 'LMantle',
   'element': 'p',
   'log': True,
   'vmin': 1e17,
   'vmax': 1e20
 }
#Specify at least 2 reservoirs for random initial conditions.
r2 = {
   'name': 'Atmosphere',
   'element': 'p',
   'log': True,
   'vmin': 1e17,
   'vmax': 1e20
}
generate_reservoirs([r1, r2], m=1e20, n=20, default="../new_config.yaml")
