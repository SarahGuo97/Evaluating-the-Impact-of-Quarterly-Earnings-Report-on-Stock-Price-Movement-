//
//  gnuplot.cpp
//  gnuplot
//
//  Created by Xiaoya Wang on 5/7/20.
//  Copyright © 2020 Xiaoya Wang. All rights reserved.
//
#include "Bootstrap.h"
#include <stdio.h>
void Bootstrap::plotResults(int dataSize){
    FILE *gnuplotPipe,*tempDataFile;
    char *tempDataFileName1, *tempDataFileName2, *tempDataFileName3;
    int i;
    tempDataFileName1 = "Beat_CAAR";
    tempDataFileName2 = "Meet_CAAR";
    tempDataFileName3 = "Miss_CAAR";
    gnuplotPipe = popen("/opt/local/bin/gnuplot --persist","w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n",tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fflush(gnuplotPipe);
        //Plot Beat
        tempDataFile = fopen(tempDataFileName1,"w");
        for (i = -dataSize; i <= dataSize; i++) {
            fprintf(tempDataFile,"%i %lf\n",i,CAAR_combined[0][i+dataSize]);
        }
        fclose(tempDataFile);
        //Plot Miss
        tempDataFile = fopen(tempDataFileName2,"w");
        for (i = -dataSize; i <= dataSize; i++) {
            fprintf(tempDataFile,"%i %lf\n",i,CAAR_combined[1][i+dataSize]);
        }
        fclose(tempDataFile);
        //Plot Meat
        tempDataFile = fopen(tempDataFileName3,"w");
        for (i = -dataSize; i <= dataSize; i++) {
            fprintf(tempDataFile,"%i %lf\n",i,CAAR_combined[2][i+dataSize]);
        }
        fclose(tempDataFile);
    } else {
        printf("gnuplot not found...");
    }
}
