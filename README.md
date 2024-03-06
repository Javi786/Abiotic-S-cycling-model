"Earth Without Life: A Global Model for Sulfur Cycling Across Deep Time" by Rafael Rianço, Javed Akhter Mondal, Henry Jurney, Marcos Jusino-Maldonado and H. James Cleaves II

Project Description:

Sulfur is a redox active element that may have helped mediate the electron flow which kickstarted life, and is a bioessential element for all life on Earth. Sulfur cycles through Earth’s major geochemical reservoirs (the biosphere, atmosphere, oceans, continental and oceanic crust, and upper and lower mantle) through a complex set of interactions which are governed by physical and chemical phenomena. Life may alter some of the fluxes between these reservoirs as it develops, thus it is of interest to explore how S fluxes might proceed in the absence of biology, all other things being equal. Understanding how these fluxes relate may help parameterize how P apportions during planetary evolution, which may provide insights regarding the evolution of planetary habitability and biological productivity, which may further relate to the likelihood of the development of life and complex life on Earth and other planetary bodies. As planets evolve over time, geochemical fluxes are fundamentally dynamical in nature, thus dynamical models provide good ways to examine how fluxes might evolve in the absence of biology.

The Program:

The model works on the basis of reservoirs and geochemical processes (modules) and how they change through time. Modifiable mass values for each reservoir (in kilograms) are modeled to exchange between themselves by different geochemical processes (in kilograms/year). Accessing the "new_config.yaml" file will show you the basic parameters used in the S cycling model. The reservoirs include: Lower Mantle, Upper Mantle, Continental Crust, Oceanic Crust, Oceans, and Atmosphere. We included many geochemical processes, which are separated into independent modules that are programmed to run simultaneously and interact with each other, exchanging masses from the reservoirs. It is important to note that the mass values must be in kilograms and that the flux values most be in kilograms/year. These modules are modifiable (in C++ language) and the program gives the user liberty to add or remove modules for new geochemical processes at convenience. You can see the basic structure of the modules by accessing "src" > "impl_modules.hpp".

Instructions:

To run the program, you must first execute "make" to gather the modified parameters and prepare them to run. If you plan to run different simulations, I suggest you execute "make clean" then "make" before running new simulations. The modules you wish to consider in the simulation must be activated in the "modules.hpp" file and must be typed under the Processes tab inside "new_config.yaml". To access the list of running modules, go to "src" > "modules" > "modules.hpp". To acti- vate the modules in the list at "modules.hpp", simply remove the # (remove comment). To inactivate the module, simply add the # (comment it). These modules also depend on certain parameters. These parameters could be basic planetary values (e.g. volume of the ocean), while others could be specific values (e.g. P that gets precipitated in the oceans). To change these parameters, type them under the Processes tab inside "new_config.yaml".

After all the parameters and modules are set, run the program by executing "./evolve new_config.yaml". The program allows different configuration settings to be used. "new_config.yaml" is simply the configuration settings for the purpose of our P cycling model. To run the model in different settings, create a new .yaml file and run the program by executing "./evolve [name of configuration file].yaml". This, however, is not necessary considering how "new_config.yaml" is easily modifiable (although, it might be useful for modeling in different planets). Advice: keep track of the changes you make in "new_config.yaml" by taking notes of them. This will help you understand where new settings might have gone wrong. If you successfully run the program, a large matrix will data will begin to output. This data will be automatically saved in a text file called "out_nfig" which should be saved inside the same folder where the code is saved in your reposito- ry. To access it, go to "output" > "out_nfig". With this output text file, you may graph the results using whichever software you are most comfortable with.

For random seedings: 0 - Adapt the paths to your own device, on the files "run_multi_config.py", and

1 - Choose reservoirs, values and number of config files to be generated on seedings.py

2 - Open terminal on main and type "make seedings"

-> this will create the directory "seedings_files" and its subfile "Results"; -> Then runs "seedings.py" and redirects the generated config files into the seedings_file directory -> Creates an evolve file -> runs "run_multi_config.py" which runs simulations for all the config-files (directed into the "output" directory)

3 - Open terminal on "output" and copy the matrix-files, results of the simulations, into the "/seedings_files/Results" directory, by typing

$ cp *.dot /home/rafael/Desktop/BMSIS_Internship2021/P-Cycle-main/seedings_files/Results $ cp *.txt /home/rafael/Desktop/BMSIS_Internship2021/P-Cycle-main/seedings_files/Results

(remember to adapt the paths) and then remove the now superfluous material on the output directory, by

$ rm *.yaml $ rm *.txt $ rm *.dot $ cd .. 4 - Create the plot by runing the .py file "generate_single_plot.py", by typing

$ python3 processing/generate_single_plot.py seedings_files/Results/*.txt -c Atmosphere0 -c CCrust0

(change the reservoirs names to obtain the ones in study) ------> If it runs smoothly, the plot should appear on the main directory
