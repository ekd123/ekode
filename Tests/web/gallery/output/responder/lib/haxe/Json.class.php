<?php

class haxe_Json {
	public function __construct(){}
	static function stringify($value, $replacer = null) {
		return haxe_Json::phpJsonEncode($value);
	}
	static function phpJsonEncode($val) {
		$json = json_encode(haxe_Json::convertBeforeEncode($val));
		if(($json === false)) {
			haxe_Json_0($json, $val);
		} else {
			return $json;
		}
	}
	static function convertBeforeEncode($val) {
		$arr = null;
		if(is_object($val)) {
			$_g = get_class($val);
			switch($_g) {
			case "_hx_anonymous":case "stdClass":{
				$arr = php_Lib::associativeArrayOfObject($val);
			}break;
			case "_hx_array":{
				$arr = php_Lib::toPhpArray($val);
			}break;
			case "Date":{
				return Std::string($val);
			}break;
			case "HList":{
				$arr = php_Lib::toPhpArray(Lambda::harray($val));
			}break;
			case "_hx_enum":{
				return haxe_Json_1($_g, $arr, $val);
			}break;
			case "StringMap":case "IntMap":{
				$arr = php_Lib::associativeArrayOfHash($val);
			}break;
			default:{
				$arr = php_Lib::associativeArrayOfObject($val);
			}break;
			}
		} else {
			if(is_array($val)) {
				$arr = $val;
			} else {
				if(is_float($val) && !is_finite($val)) {
					$val = null;
				}
				return $val;
			}
		}
		return array_map((isset(haxe_Json::$convertBeforeEncode) ? haxe_Json::$convertBeforeEncode: array("haxe_Json", "convertBeforeEncode")), $arr);
	}
	function __toString() { return 'haxe.Json'; }
}
function haxe_Json_0(&$json, &$val) {
	throw new HException("invalid json");
}
function haxe_Json_1(&$_g, &$arr, &$val) {
	{
		$e = $val;
		return $e->index;
	}
}
