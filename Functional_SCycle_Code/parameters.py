from generate_configs import generate_reservoirs, generate_parameters

p1 = {
   'process': 'Subduction_Volcanism_Rainout',
   'parameter': 'atm_timescale',
   'log': True,
   'vmin': 1e-3,
   'vmax': 1e3
 }

generate_parameters([p1], n=50, default="./new_config.yaml")
