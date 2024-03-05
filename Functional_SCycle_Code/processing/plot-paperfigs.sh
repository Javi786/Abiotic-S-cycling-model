FOLDER=../output/hifix/
# FIG3. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -c CCrust2 \
                                            -l 'Atmosphere' -l 'C. Crust' \
                                            -e  -o FIG3A.eps -n '(a)'
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -c UMantle2 \
                                            -l 'Lower mantle'  -l 'Upper mantle'  \
                                            -e  -o FIG3B.eps -n '(b)'
python generate_single_plot.py $FOLDER*.txt -c Oceans1     -e  -o FIG3C.eps -n '(c)'
python generate_single_plot.py $FOLDER*.txt -c Oceans2         -o FIG3D.eps -n '(d)'
#
FOLDER=../output/lofix/
## FIG4. Complete evolution -- reservoirs
python generate_single_plot.py $FOLDER*.txt -c Atmosphere0 -c CCrust2 \
                                            -l 'Atmosphere' -l 'C. Crust' \
                                            -e  -o FIG4A.eps -n '(a)'
python generate_single_plot.py $FOLDER*.txt -c LMantle2 -c UMantle2 \
                                            -l 'Lower mantle'  -l 'Upper mantle'  \
                                            -e  -o FIG4B.eps -n '(b)'
python generate_single_plot.py $FOLDER*.txt -c Oceans1     -e  -o FIG4C.eps -n '(c)'
python generate_single_plot.py $FOLDER*.txt -c Oceans2         -o FIG4D.eps -n '(d)'

## FIG5. Fixes point approach
FOLDER=../output/fixed-point/
python generate_single_plot.py $FOLDER/*.txt -c Atmosphere0 -c UMantle2 -p -o FIG5A.eps

FOLDER=../output/
## FIG6. Main parameters influence
python generate_pdependence.py -f $FOLDER/manmix-25/ \
                               -f $FOLDER/manmix-50/ \
                               -f $FOLDER/manmix-75/ \
                               -l 25 -l 50 -l 75 -c Atmosphere0 -o FIG6A.eps -n '(a)'

python generate_pdependence.py -f $FOLDER/ocemix-25/ \
                               -f $FOLDER/ocemix-50/ \
                               -f $FOLDER/ocemix-75/ \
                               -l 25 -l 50 -l 75 -c Atmosphere0 -o FIG6B.eps -n '(b)'

FOLDER=../output/man-ox/
# FIG7. Main parameters influence
python generate_pdependence.py -f $FOLDER  -c Atmosphere0 -o FIG7A.eps -n '(a)'
python generate_pdependence.py -f $FOLDER  \
                               -c Volcanism0 -c Volcanism1 -c Volcanism2 \
                               -l arc-like -l MORB-like -l hotspot-like \
                               -o FIG7B.eps -n '(b)'

FOLDER=../output/
python generate_pdependence.py -f $FOLDER/accretion-25/ \
                               -f $FOLDER/accretion-50/ \
                               -f $FOLDER/accretion-75/ \
                               -l 25 -l 50 -l 75 -c Atmosphere0 -o FIG8A.eps -n '(a)'

python generate_pdependence.py -f $FOLDER/subrate-050/ \
                               -f $FOLDER/subrate-100/ \
                               -f $FOLDER/subrate-150/ \
                               -l '50 Ma' -l '100 Ma' -l '150 Ma' -c Atmosphere0 -o FIG8B.eps -n '(b)'
