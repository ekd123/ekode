import php.Lib;
import sys.FileSystem;
import haxe.Json;
import StringTools;

// get location: /responder
class Responder {
	static public function main () {
		var files = FileSystem.readDirectory("../images");
		var validimages = new Array<String>();
		for(it in files) {
			if(StringTools.endsWith(it, ".jpg") == true)
				validimages.push(it);
			else if(StringTools.endsWith(it, ".png") == true)
				validimages.push(it);
			else if(StringTools.endsWith(it, ".gif") == true)
				validimages.push(it);
		}
		php.Lib.println(Json.stringify(validimages));
	}
}
