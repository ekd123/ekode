<?php

class php_Lib {
	public function __construct(){}
	static function hprint($v) {
		echo(Std::string($v));
	}
	static function println($v) {
		php_Lib::hprint($v);
		php_Lib::hprint("\x0A");
	}
	static function toPhpArray($a) {
		return $a->a;
	}
	static function associativeArrayOfHash($hash) {
		return $hash->h;
	}
	static function associativeArrayOfObject($ob) {
		return (array) $ob;
	}
	function __toString() { return 'php.Lib'; }
}
