#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void prepare_output_file(const string output_filename);

void help() {
    cout << "This program will create a csv file in order to store data that can be used for Machine Learning." << endl;
    cout << "It creates a file and set the different columns" << endl;
    cout << "One argument must be given : it is the name of the csv file (path is optional in the name)" << endl;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        help();
        return 1;
    }
    string output_filename = argv[1];
    if(output_filename.rfind(".csv") == string::npos) {
        cout << "The given argument is not valid" << endl;
        return 2;
    }
    prepare_output_file(output_filename);

    return 0;
}


void prepare_output_file(const string output_filename)
{
    ofstream output_file;
    output_file.open(output_filename.c_str(), ios::out);

    if(output_file.is_open())
    {
        output_file << "identifier" << "," << "background.identifier" << ",";
        output_file << "vehicle1.identifier" << "," << "vehicle1.trajectory" << "," << "vehicle1.appearance_change" << "," << "vehicle1.shadowed" << "," << "vehicle1.dust" << ",";
        output_file << "vehicle2.identifier" << "," << "vehicle2.trajectory" << "," << "vehicle2.appearance_change" << "," << "vehicle2.shadowed" << "," << "vehicle2.dust" << ",";
        output_file << "vehicle3.identifier" << "," << "vehicle3.trajectory" << "," << "vehicle3.appearance_change" << "," << "vehicle3.shadowed" << "," << "vehicle3.dust" << ",";
        output_file << "vehicle4.identifier" << "," << "vehicle4.trajectory" << "," << "vehicle4.appearance_change" << "," << "vehicle4.shadowed" << "," << "vehicle4.dust" << ",";
        output_file << "vehicle5.identifier" << "," << "vehicle5.trajectory" << "," << "vehicle5.appearance_change" << "," << "vehicle5.shadowed" << "," << "vehicle5.dust" << ",";

        output_file << "man1.identifier" << "," << "man1.trajectory" << "," << "man1.appearance_change" << "," << "man1.shadowed" << ",";
        output_file << "man2.identifier" << "," << "man2.trajectory" << "," << "man2.appearance_change" << "," << "man2.shadowed" << ",";
        output_file << "man3.identifier" << "," << "man3.trajectory" << "," << "man3.appearance_change" << "," << "man3.shadowed" << ",";
        output_file << "man4.identifier" << "," << "man4.trajectory" << "," << "man4.appearance_change" << "," << "man4.shadowed" << ",";
        output_file << "man5.identifier" << "," << "man5.trajectory" << "," << "man5.appearance_change" << "," << "man5.shadowed" << ",";

        output_file << "distractors.butterfly_level" << "," << "distractors.bird_level" << "," << "distractors.far_moving_vegetation" << "," << "distractors.close_moving_vegetation" << "," << "distractors.light_reflexion" << "," << "distractors.blinking_light" << ",";

        output_file << "occultants.solid_level" << "," << "occultants.semi_transparent_level" << ",";

        output_file << "camera.vibration" << "," << "camera.focal_change" << "," << "camera.pan_motion" << "," << "camera.tilt_motion" << "," << "camera.altitude" << ",";
        output_file << "capture.illumination_level" << "," << "capture.global_light_change_level" << "," << "capture.local_light_change_level" << "," << "capture.heat_haze_level" << "," << "capture.humidity_level" << "," << "capture.fog_level" << ",";

        output_file << "signal_quality.picture_width" << "," << "signal_quality.picture_height" << "," << "signal_quality.force_balance" << "," << "signal_quality.luminance_mean" << "," << "signal_quality.luminance_dev" << "," << "signal_quality.chrominance_U_mean" << "," << "signal_quality.chrominance_U_dev" << "," << "signal_quality.chrominance_V_mean" << "," << "signal_quality.chrominance_V_dev" << "," << "signal_quality.blur_level" << "," << "signal_quality.static_noise_level" << "," << "signal_quality.dynamic_noise_level" << "," << "signal_quality.frame_instability" << "," << "signal_quality.line_instability" << "," << "signal_quality.compression_artefact_level" << ",";

        output_file << "label" << "\n";

        output_file.close();
    }
    else
    {
        cout << "file not opened" << endl;
    }

}
