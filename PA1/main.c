#include "functions.h"

int main(void){

    // open file and check if opened
    FILE* infile = fopen("FitbitData.csv", "r");
    if (!infile) {
        printf("Can't open file\n");
        return 0;
    }

    // open output file
    FILE* outfile = fopen("Results.csv", "w");

    // person wearing the fitbit
    FitbitData person1;

    // variables for counting rows and the values in each row
    char buf[1440];
    int row_num = 0, field_num = 0, field_count = 0;

    // variables used for totaling data
    double total_cal = 0, total_distance = 0, avg_hr_rate = 0;
    int total_floors = 0, total_steps = 0, max_steps = 0, total_worst_sleep = 0, current_sleep = 0;

    char max_steps_time[10];

    // read csv file
    while (fgets(buf, sizeof(buf), infile)) {
       
        row_num++;
        field_num = 0;

        // bypass the collumn titles in the csv file
        if (row_num == 1) {
            continue;
        }

        // tokenize each field in each line by comma separation
        char* field = strtok(buf, ",");

        // while the current field doesnt contain a null value
        while (field != '\0') {

            // match data points to the corresponding field
            // field num is the collumn containing each corresponding data type in FitbitData
            if (field_num == 0) {

                // copy current field to the patient data
                strcpy(person1.patient, field);
            }
            if (field_num == 1) {

                strcpy(person1.minute, field);
            }
            if (field_num == 2) {

                person1.calories = atof(field);
                // keep track of total calories
                total_cal += person1.calories;
            }
            if (field_num == 3) {

                person1.distance = atof(field);
                total_distance += person1.distance;
            }
            if (field_num == 4) {

                person1.floors = atoi(field);
                total_floors += person1.floors;
            }
            if (field_num == 5) {

                person1.heartRate = atoi(field);
                // keep track of average heartrate, will be divided by total rows later
                avg_hr_rate += person1.heartRate;
            }
            if (field_num == 6) {

                person1.steps = atoi(field);
                total_steps += person1.steps;

                // check for max step and latest time the max steps occured at
                if (max_steps <= person1.steps) {
                    max_steps = person1.steps;
                    strcpy(max_steps_time, person1.minute);
                }
            }
            if (field_num == 7) {

                person1.sleepLevel = atoi(field);

                // if the next minute contains equal or worse sleep, add to total worst sleep
                if (person1.sleepLevel >= current_sleep) {
                    total_worst_sleep += person1.sleepLevel;
                }
                
                // person is awake at a level greater than 1
                if (person1.sleepLevel > 1) {

                    // flag current sleep level
                    current_sleep = person1.sleepLevel;
                }
            }
            field = strtok(NULL, ",");
            field_num++;
        }
    }

    // calculate average hr rate
    avg_hr_rate = avg_hr_rate / 1440;

    // print to Results.csv
    fprintf(outfile, "Total Calories, Total Distance, Total Floors, Avg Heartrate, Max Steps, Sleep\n");  
    fprintf(outfile, "%lf, %lf, %d, %lf, %d, %d",total_cal, total_distance,
        total_floors, avg_hr_rate, max_steps, total_worst_sleep);

    // print results to screen
    printf("Total Calories, Total Distance, Total Floors, Avg Heartrate, Max Steps, Sleep\n");
    printf("%lf, %lf, %d, %lf, %d, %d", total_cal, total_distance, total_floors, avg_hr_rate, 
        max_steps, total_worst_sleep);
 
    // close files
    fclose(infile);
    fclose(outfile);

    return 0;
}
