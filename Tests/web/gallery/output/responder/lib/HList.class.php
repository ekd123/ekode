<?php

class HList implements IteratorAggregate{
	public function __construct(){}
	public function getIterator() {
		return $this->iterator();
	}
	public function iterator() {
		return new _hx_list_iterator($this);
	}
	function __toString() { return 'List'; }
}
