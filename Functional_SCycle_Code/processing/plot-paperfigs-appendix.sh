FOLDER=../output/appendix-subrate/
## FIG3. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -n '(a)'\
                                            -l '100 Ma' -l '150 Ma' -l '50 Ma' -e  -o FIGA2A.eps
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -n '(b)'\
                                            -l '100 Ma' -l '150 Ma' -l '50 Ma' -e  -o FIGA2B.eps
python generate_single_plot.py $FOLDER*.txt -c Oceans1 -n '(c)'\
                                            -l '100 Ma' -l '150 Ma' -l '50 Ma' -e  -o FIGA2C.eps
python generate_single_plot.py $FOLDER*.txt -c Oceans2 -n '(d)'\
                                            -l '100 Ma' -l '150 Ma' -l '50 Ma' -o FIGA2D.eps

FOLDER=../output/appendix-ocvol/
## FIG3. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -n '(a)'\
                                            -l '-40 pc.' -l constant -l '+40 pc.' -e  -o FIGA3A.eps
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -n '(b)'\
                                            -l '-40 pc.' -l constant -l '+40 pc.' -e  -o FIGA3B.eps
python generate_single_plot.py $FOLDER*.txt -c Oceans1 -n '(c)'\
                                            -l '-40 pc.' -l constant -l '+40 pc.' -e  -o FIGA3C.eps
python generate_single_plot.py $FOLDER*.txt -c Oceans2 -n '(d)'\
                                            -l '-40 pc.' -l constant -l '+40 pc.'     -o FIGA3D.eps

FOLDER=../output/appendix-hifix-BSE93/
# FIG3. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -c CCrust2 \
                                            -l 'Atmosphere' -l 'C. Crust' \
                                            -e  -o FIGA4A.eps -n '(a)'
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -c UMantle2 \
                                            -l 'Lower mantle'  -l 'Upper mantle'  \
                                            -e  -o FIGA4B.eps -n '(b)'
python generate_single_plot.py $FOLDER*.txt -c Oceans1     -e  -o FIGA4C.eps -n '(c)'
python generate_single_plot.py $FOLDER*.txt -c Oceans2         -o FIGA4D.eps -n '(d)'

FOLDER=../output/appendix-lofix-BSE93/
# FIG3. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -c CCrust2 \
                                            -l 'Atmosphere' -l 'C. Crust' \
                                            -e  -o FIGA5A.eps -n '(a)'
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -c UMantle2 \
                                            -l 'Lower mantle'  -l 'Upper mantle'  \
                                            -e  -o FIGA5B.eps -n '(b)'
python generate_single_plot.py $FOLDER*.txt -c Oceans1     -e  -o FIGA5C.eps -n '(c)'
python generate_single_plot.py $FOLDER*.txt -c Oceans2         -o FIGA5D.eps -n '(d)'
