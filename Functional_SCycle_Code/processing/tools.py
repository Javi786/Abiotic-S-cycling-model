import pandas as pd

class Axis:
    def __init__(self, norm, ylabel, earthscale=1):
        self.norm = norm
        self.ylabel = ylabel
        self.earthscale = earthscale

class Parameters:
    # TODO: define a default set of values when an unknown category is accessed
    def __init__(self):
        PARAMS = {}
        PARAMS['default'] = Axis(1, 'undefined axis')

        PARAMS['Atmosphere0'] = Axis(1, 'Atmosphere S mass [kg]', earthscale = 0)
        PARAMS['Atmosphere1'] = Axis(1, 'Atmosphere Oxidised S mass [kg]', earthscale = 3.6e9)
        PARAMS['Oceans0'] = Axis(1, 'Oceans S mass [kg]', earthscale = 4.3e12) #from M ocean Concentrations of Sred
        PARAMS['Oceans1'] = Axis(1, 'Oceans S mass [kg]', earthscale = 1.3e18) #from M ocean Concentrations of Sox
        PARAMS['LMantle0'] = Axis(1, 'Mantle Reduced S mass [kg]', earthscale= 15e20)
        PARAMS['LMantle1'] = Axis(1, 'Mantle S mass [kg]', earthscale= 0) 
        PARAMS['UMantle0'] = Axis(1, 'Mantle Reduced S mass [kg]', earthscale= 5.5e20) 
        PARAMS['UMantle1'] = Axis(1, 'Mantle S mass [kg]', earthscale= 0)
        PARAMS['OCrust0'] = Axis(1, 'Ocean Crust S mass [kg]', earthscale= 3.4e18) #from Mtotal(OC) = 9E21kg and 900ppm of Sred
        PARAMS['OCrust1'] = Axis(1, 'Ocean Crust S mass [kg]', earthscale= 4.8e17) #from Mtotal(OC) = 9E21kg and 100ppm of Sred
        PARAMS['CCrust0'] = Axis(1, 'Continental Crust S mass [kg]', earthscale= 1.8e19) #from Mtotal(CC) = 2E22kg and 540ppm of Sred
        PARAMS['CCrust1'] = Axis(1, 'Continental Crust S mass [kg]', earthscale= 6.20e18) #from Mtotal(CC) = 2E22kg and 60ppm of Sox
        PARAMS['Sediments0'] = Axis(1, 'Marine Sediments S mass [kg]', earthscale= 8.8e17) #from Mtotal(sed) = 2.6E20kg and 3200ppm of Sred
        PARAMS['Sediments1'] = Axis(1, 'Marine Sediments S mass [kg]', earthscale= 2.2e17) #from Mtotal(sed) = 2.6E20kg and 800ppm of Sox
        

        PARAMS['Volcanism0'] = Axis(1, 'Volcanic flux of P [kg $yr^{-1}$]', earthscale=3.73e9)
        PARAMS['Volcanism1'] = Axis(1, 'Volcanic flux of P [kg $yr^{-1}$]', earthscale=1.49e10)
        PARAMS['Volcanism2'] = Axis(1, 'Volcanic flux of P [kg $yr^{-1}$]', earthscale=2.5e9)
        PARAMS['Subduction0'] = Axis(1, 'Subduction flux of P [kg/yr]', earthscale=8.221e10)
        PARAMS['Subduction1'] = Axis(1, 'Accretionn flux of P [kg $yr^{-1}$]', earthscale=1.209e10)
        PARAMS['Convection0'] = Axis(1, 'Convection flux of P [kg/yr]', earthscale=2.41e13)
        #PARAMS['Henry0'] = Axis(1, 'Ocean-Atmosphere equilibration flux [kg/yr]', earthscale=1)
        #PARAMS['AbioticFixation0'] = Axis(1, 'Abiotic fixation to NO$_x$ [kg/yr]', earthscale=1)
        #PARAMS['AbioticFixation1'] = Axis(1, 'Abiotic fixation to NH$_x$ [kg/yr]', earthscale=1)
        PARAMS['HydrothermalCirculation0'] = Axis(1, 'HT. circulation of P$ [kg/yr]', earthscale=4.4e8)
        #PARAMS['HydrothermalCirculation1'] = Axis(1e6, 'HT. circulation from N$_2$ [1e6 kg/yr]', earthscale=1)
        #PARAMS['FreundlichAdsorption0'] = Axis(1e10, 'Freundlich adsorption [1e10 kg/yr]', earthscale=1)
        PARAMS['Erosion0'] = Axis(1, 'Erosion rate [kg/yr]', earthscale=3.5e10)
        PARAMS['Erosion1'] = Axis(1, 'Erosion rate [kg/yr]', earthscale=3.5e10)
        PARAMS['Erosion5'] = Axis(1, 'Erosion rate [kg/yr]', earthscale=3.5e10)
        #PARAMS['Erosion1'] = Axis(1, 'Erosion rate into Ocean [kg/yr]', earthscale=3.5e10)
        PARAMS['Erosion2'] = Axis(1, 'Sedimentation Rate [kg/yr]', earthscale=3.5e10)
        PARAMS['Erosion3'] = Axis(1, 'Oceans Volume [dm³]', earthscale=1.35e21)
        #PARAMS['Erosion1'] = Axis(1, 'Relative continental area', earthscale=0.35)
        PARAMS['WindErosion0'] = Axis(1, 'Erosion rate [1e10 kg/yr]', earthscale=2.07e9)
        PARAMS['Precipitation0'] = Axis(1, 'Precipitation Rate [kg/yr]', earthscale=2.8e9)

#        PARAMS['Atmosphere2'] = {'norm':1, 'ylabel':'N-content [kg]', 'isLog':True, 'Earth':1e15}
#        PARAMS['BioticContribution0'] = {'norm':1, 'ylabel':'N-content [kg]', 'isLog':True, 'Earth':1}
#        PARAMS['BioticContribution1'] = {'norm':1, 'ylabel':'Synthesis pathway', 'isLog':False, 'Earth':1}
#        PARAMS['BioticContribution2'] = {'norm':1, 'ylabel':'Assimil. pathway', 'isLog':False, 'Earth':1}
        PARAMS['CometDelivery0'] = Axis(1, 'ET material input rate [kg/yr]', earthscale=2e5)
        
        PARAMS['Subduction_Volcanism_Rainout4'] = Axis(1, 'Flux into CC [kg/yr]', earthscale=3.5e10) # Accretion -> CC Sred
        PARAMS['Subduction_Volcanism_Rainout5'] = Axis(1, 'Flux into CC [kg/yr]', earthscale=3.5e10) # Accretion -> CC Sox
        PARAMS['Subduction_Volcanism_Rainout6'] = Axis(1, 'Flux into CC [kg/yr]', earthscale=3.5e10) # Arc Volcanism -> CC Sred
        PARAMS['Subduction_Volcanism_Rainout9'] = Axis(1, 'Flux into CC [kg/yr]', earthscale=3.5e10) # Hotspot Volcsnism -> CC Sred
        PARAMS['Subduction_Volcanism_Rainout10'] = Axis(1, 'Flux into CC [kg/yr]', earthscale=3.5e10) # ATM Sox -> CC Sox


        
#        PARAMS['Impacts0'] = {'norm':1, 'ylabel':'Assimil. pathway', 'isLog':True, 'Earth':1}

        self.params = PARAMS

def loadfile(fname, columns, PARAMS, xaxis='time'):
    if type(columns) != list:
        columns = [columns]

    try:
        data = pd.read_csv(fname)
    except:
        data = pd.DataFrame()

    if not sum([x in data.columns for x in columns]):
        print([data.columns])
        print(columns)
        print("File doesn't seem to be an output file, skipping", fname)
        return None, None

    xa = data[xaxis].values[1:]
    if xaxis != 'time':
        xa/= PARAMS[columns[0]].norm

    ya = []
    for col in columns:
        print(data[col])
        y = data[col].values[1:]
        print("y= ", y)
        print("PARAMS COL= ", PARAMS[col].norm)
        y /= PARAMS[col].norm
        print("Params= ", PARAMS[col].norm)
        print("y= ", y)
        ya.append(y)

    print(fname)
    return xa, ya
