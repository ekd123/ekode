import haxe.io.Eof;
import sys.io.File;

class Demo {
	static function main () {
		var xml = Xml.parse (sys.io.File.getContent (Sys.args()[0]));
		for (elt in xml.elements ())
		{
			Sys.println (elt.nodeName);
			for (elt in elt.elementsNamed("url"))
			{
				Sys.println ("\t" + elt.firstChild().toString());
			}
		}
	}
}

