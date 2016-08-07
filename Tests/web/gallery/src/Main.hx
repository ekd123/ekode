import js.Lib;
import js.html.CSSStyleDeclaration;
import js.html.Document;
import js.html.EventListener;
import js.html.XMLHttpRequest;
import js.Browser;

class Main {
	static public function main () {
        var req = new XMLHttpRequest();
        req.open("GET", "/gallery/responder?t=" + Math.random(), true);
        req.send();
        req.onreadystatechange = function (e : EventListener) {
            if (req.readyState != 4 || req.status != 200) {
                return;
            }
            var array : Array<String> = haxe.Json.parse(req.responseText);
            for (it in array) {
                var div = Browser.document.getElementById("gallery");
                var img = Browser.document.createImageElement();
                img.src = "images/" + it;
                img.className = "photo";
                img.width = 300;
                div.appendChild(img);
            }
        };
	}
}
