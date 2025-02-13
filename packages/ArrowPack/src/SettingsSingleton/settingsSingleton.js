"use strict";

const fs = require("fs");

class settingsSingleton {
	constructor(json) {
		this.settings = { entry: "src", exit: "public", autoClear: false };

		/*if (fs.existsSync("./lib")) {
			this.settings.entry = "lib";
		}*/
		if (fs.existsSync("./dist")) {
			this.settings.exit = "dist";
		} else if (fs.existsSync("./build")) {
			this.settings.exit = "build";
		}

		if (json !== undefined && json !== null) {
			json = JSON.parse(json);
			for (var key in json) {
				if (this.settings[key] !== undefined) {
					this.settings[key] = json[key];
				}
			}
		}
	}
	getValue(key, required) { // Will throw an error if the key is not present and required is true
		if (this.settings[key] !== undefined) { return this.settings[key]; } else { if (required === true) { throw "Error: could not find required key :("; } else { return null; } }
	}
}

module.exports = settingsSingleton;