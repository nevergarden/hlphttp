## HLPHTTP
Hashlink bindings for picohttpparser.

For now only request parser is supported.

## Installation
Install using:  
```sh
git clone --recurse-submodules https://github.com/nevergarden/hlphttp
haxelib dev hlphttp hlphttp
# Now build the binaries:
cd hlphttp
mkdir build
cd build
cmake ..
make
# Copy picohttp.hdll where your project or hashlink is:
sudo cp picohttp.hdll /usr/lib
```

Add to project:
```hxml
--library hlphttp
```

## Usage
```haxe
import picohttp.PicoHttpParser;

class Main {
	public static function main() {
		// Get this from socket but for now use string:
		var reqString = "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\nHost: www.kittyhell.com\r\n\r\n";
		var parsedRequest = PicoHttpParser.parseRequest(reqString);
		trace(parsedRequest);
	}
}
```