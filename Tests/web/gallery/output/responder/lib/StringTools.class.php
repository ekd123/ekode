<?php

class StringTools {
	public function __construct(){}
	static function endsWith($s, $end) {
		$elen = strlen($end);
		$slen = strlen($s);
		return $slen >= $elen && _hx_substr($s, $slen - $elen, $elen) === $end;
	}
	function __toString() { return 'StringTools'; }
}
