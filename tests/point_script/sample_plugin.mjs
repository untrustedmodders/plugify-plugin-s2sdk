import { Plugin } from 'plugify';
// We could not load module here as v8 plugify load much ealier
//import { Instance } from "cs_script/point_script";
import * as s2sdk from ':s2sdk';

export class SamplePlugin extends Plugin {
	pluginStart() {
		console.log("SamplePlugin");

		s2sdk.OnEntityCreated_Register(SamplePlugin.OnEntityCreated)
	}

	pluginEnd() {
		s2sdk.OnEntityCreated_Unregister(SamplePlugin.OnEntityCreated);
	}

	static OnEntityCreated(entityHandle) {
		var className = s2sdk.GetEntityClassname(entityHandle)
		if (className == "point_script") {
			console.log("SamplePlugin: " + className + " created");
			console.log("SamplePlugin: [handle]" + entityHandle + " created");
			setTimeout(() => {
				console.log("SamplePlugin: [timer]");
				// We could only load module dynamicly after point script created
				import("cs_script/point_script").then(point_script => {
					var { Instance } = point_script;
					console.log("cs_script point_script - Imported");
					Instance.Msg("cs_script point_script - Imported");
					
					Instance.SetThink(() => {
					  Instance.Msg("test think");

					  Instance.SetNextThink(Instance.GetGameTime());
					})
					Instance.SetNextThink(Instance.GetGameTime());
				});
			}, 100.0);
		}
	}
}