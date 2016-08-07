<?php

class Responder {
	public function __construct(){}
	static function main() {
		$files = sys_FileSystem::readDirectory("../images");
		$validimages = new _hx_array(array());
		{
			$_g = 0;
			while($_g < $files->length) {
				$it = $files[$_g];
				++$_g;
				if(StringTools::endsWith($it, ".jpg") === true) {
					$validimages->push($it);
				} else {
					if(StringTools::endsWith($it, ".png") === true) {
						$validimages->push($it);
					} else {
						if(StringTools::endsWith($it, ".gif") === true) {
							$validimages->push($it);
						}
					}
				}
				unset($it);
			}
		}
		php_Lib::println(haxe_Json::stringify($validimages, null));
	}
	function __toString() { return 'Responder'; }
}
