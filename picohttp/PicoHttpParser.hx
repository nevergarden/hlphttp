package picohttp;

typedef ParsedRequest = {
	var method:String;
	var path:String;
	var request_size:Int;
	var minor_verison:Int;
	var header_count:Int;
	var headers:Map<String,String>;
}

abstract class NativeParsedRequest {
	public var method:hl.Bytes;
	public var path:hl.Bytes;
	public var request_size:Int;
	public var minor_version:Int;
	public var header_count:Int;
	public var headers:hl.NativeArray<Dynamic>;
}

@:hlNative("picohttpparser")
class PicoHttpParser {
	public static function parseRequest(request:String, ?max_headers:Int=100):ParsedRequest {
		var headers : Map<String,String> = new Map<String, String>();
		var buffer = hl.Bytes.fromBytes(haxe.io.Bytes.ofString(request));
		var data = parse_request(buffer, max_headers);

		for(header in data.headers) {
			headers.set( @:privateAccess String.fromUTF8(header.name), 
				@:privateAccess String.fromUTF8(header.value) );
		}

		var ret : ParsedRequest = {
			method: @:privateAccess String.fromUTF8(data.method),
			path: @:privateAccess String.fromUTF8(data.path),
			request_size: data.request_size,
			minor_verison: data.minor_version,
			header_count: data.header_count,
			headers: headers
		};
		free_pico_data(data);
		return ret;
	}

	@:hlNative("?picohttpparser", "parse_request")
	static function parse_request(buffer:hl.Bytes, max_header:Int):NativeParsedRequest {return null;}

	@:hlNative("?picohttpparser", "free_pico_data")
	static function free_pico_data(parsed_data:NativeParsedRequest):Void {}
}
