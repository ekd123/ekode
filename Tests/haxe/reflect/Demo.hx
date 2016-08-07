class Demo {
	static function Foo () {
		Sys.println ("In Foo");
	}
	static function main () {
		Sys.print ("has main field? ");
		Sys.println (Reflect.hasField(Demo, "main"));
		Sys.print ("Now we are ");
		Reflect.callMethod(Demo, Reflect.field(Demo, "Foo"), []);
	}
}

