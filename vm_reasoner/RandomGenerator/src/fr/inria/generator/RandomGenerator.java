package fr.inria.generator;

import java.util.Collection;
import java.util.LinkedList;

import fr.inria.domain.Configuration;

public class RandomGenerator {
	
	public static Collection<Configuration> generateSetOfConfigurations(int numberOfConf) {

		Collection<Configuration> res = new LinkedList<Configuration>();
		for (int i = 1; i <= numberOfConf; i++) {
			res.add(generateConfiguration(i));
		}
		return res;
	}

	public static Configuration generateConfiguration(int n) {

		Configuration conf = new Configuration();
	//	System.err.println("Generating the configuration " + n);
		// Sequence descriptors
		conf.add("sequence.identifier", n);
		conf.add("sequence.comment", 0d);
		// Background selection :
		conf.add("background.identifier", 0d);
		// Targets : vehicle1
		conf.add("vehicle1.identifier", Math.floor(Math.random() * 16));
		conf.add("vehicle1.trajectory", Math.floor(Math.random() * 8));
		conf.add("vehicle1.appearance_change", 0d);
		conf.add("vehicle1.shadowed", 0d);
		conf.add("vehicle1.dust", 0d);
		// Targets : vehicle2
		conf.add("vehicle2.identifier", Math.floor(Math.random() * 16));
		conf.add("vehicle2.trajectory", Math.floor(Math.random() * 8));
		conf.add("vehicle2.appearance_change", 0d);
		conf.add("vehicle2.shadowed", 0d);
		conf.add("vehicle2.dust", 0d);
		// Targets : vehicle3
		conf.add("vehicle3.identifier", Math.floor(Math.random() * 16));
		conf.add("vehicle3.trajectory", Math.floor(Math.random() * 8));
		conf.add("vehicle3.appearance_change", 0d);
		conf.add("vehicle3.shadowed", 0d);
		conf.add("vehicle3.dust", 0d);
		// Targets : vehicle4
		conf.add("vehicle4.identifier", Math.floor(Math.random() * 16));
		conf.add("vehicle4.trajectory", Math.floor(Math.random() * 8));
		conf.add("vehicle4.appearance_change", 0d);
		conf.add("vehicle4.shadowed", 0d);
		conf.add("vehicle4.dust", 0d);
		// Targets : vehicle5
		conf.add("vehicle5.identifier", Math.floor(Math.random() * 16));
		conf.add("vehicle5.trajectory", Math.floor(Math.random() * 8));
		conf.add("vehicle5.appearance_change", 0d);
		conf.add("vehicle5.shadowed", 0d);
		conf.add("vehicle5.dust", 0d);
		// Targets : man1
		conf.add("man1.identifier", 0d);
		conf.add("man1.trajectory", 0d);
		conf.add("man1.appearance_change", 0d);
		conf.add("man1.shadowed", 0d);
		// Targets : man2
		conf.add("man2.identifier", 0d);
		conf.add("man2.trajectory", 0d);
		conf.add("man2.appearance_change", 0d);
		conf.add("man2.shadowed", 0d);
		// Targets : man3
		conf.add("man3.identifier", 0d);
		conf.add("man3.trajectory", 0d);
		conf.add("man3.appearance_change", 0d);
		conf.add("man3.shadowed", 0d);
		// Targets : man4
		conf.add("man4.identifier", 0d);
		conf.add("man4.trajectory", 0d);
		conf.add("man4.appearance_change", 0d);
		conf.add("man4.shadowed", 0d);
		// Targets : man5
		conf.add("man5.identifier", 0d);
		conf.add("man5.trajectory", 0d);
		conf.add("man5.appearance_change", 0d);
		conf.add("man5.shadowed", 0d);
		// Distractors
		conf.add("distractors.butterfly_level", Math.random() * Math.random());
		conf.add("distractors.bird_level", Math.random() * Math.random() * Math.random());
		conf.add("distractors.far_moving_vegetation", Math.random());
		conf.add("distractors.close_moving_vegetation", Math.random());
		conf.add("distractors.light_reflection", 0d);
		conf.add("distractors.blinking_light", 0d);
		// Occulting objects
		conf.add("occultants.solid_level", Math.random());
		conf.add("occultants.semi_transparent_level", Math.random());
		// Image capture conditions
		conf.add("camera.vibration", 0d);
		conf.add("camera.focal_change", 0d);
		conf.add("camera.pan_motion", 0d);
		conf.add("camera.tilt_motion", 0d);
		conf.add("camera.altitude", 1.2);

		double illumination_level = Math.sqrt(Math.random());
		conf.add("capture.illumination_level", illumination_level);
		conf.add("capture.global_light_change_level", Math.random() * Math.random());
		conf.add("capture.local_light_change_level", Math.random() * Math.random());

		if (illumination_level > 0.9) {
			conf.add("capture.heat_haze_level", Math.random() * Math.random());
		} else {
			conf.add("capture.heat_haze_level", 0d);
		}
		conf.add("capture.humidity_level", 0d);
		if ((illumination_level < 0.6) && (Math.random() < 0.05)) {
			conf.add("capture.fog_level", Math.random());
		} else {
			conf.add("capture.fog_level", 0d);
		}
		// Signal quality
		conf.add("signal_quality.picture_width", 1280d);
		conf.add("signal_quality.picture_height", 720d);
		if ((illumination_level > 0.5) && (Math.random() < 0.05)) {
			conf.add("signal_quality.force_balance", 1d);
		} else {
			conf.add("signal_quality.force_balance", 0d);
		}
		conf.add("signal_quality.luminance_mean", 128 + (Math.random() - 0.5) * 96);
		conf.add("signal_quality.luminance_dev", Math.random() * 96);
		conf.add("signal_quality.chrominance_U_mean", 128 + (Math.random() - 0.5) * 32);
		conf.add("signal_quality.chrominance_U_dev", Math.random() * 32);
		conf.add("signal_quality.chrominance_V_mean", 128 + (Math.random() - 0.5) * 32);
		conf.add("signal_quality.chrominance_V_dev", Math.random() * 32);

		if (Math.random() < 0.05) {
			conf.add("signal_quality.blur_level", Math.random());
		} else {
			conf.add("signal_quality.blur_level", 0d);
		}

		if (Math.random() < 0.05) {
			conf.add("signal_quality.static_noise_level", Math.random());
		} else {
			conf.add("signal_quality.static_noise_level", 0d);
		}

		if (Math.random() < 0.05) {
			conf.add("signal_quality.dynamic_noise_level", Math.random());
		} else {
			conf.add("signal_quality.dynamic_noise_level", 0d);
		}

		if (Math.random() < 0.05) {
			conf.add("signal_quality.frame_instability", Math.random());
		} else {
			conf.add("signal_quality.frame_instability", 0d);
		}

		if (Math.random() < 0.05) {
			conf.add("signal_quality.line_instability", Math.random());
		} else {
			conf.add("signal_quality.line_instability", 0d);
		}

		if (Math.random() < 0.05) {
			conf.add("signal_quality.compression_artefact_level", Math.random());
		} else {
			conf.add("signal_quality.compression_artefact_level", 0d);
		}

		return conf;
	}
}
