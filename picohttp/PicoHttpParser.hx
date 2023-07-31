package picohttp;

class ParsedRequest {
	public var method:hl.Bytes;
	public var path:hl.Bytes;
	public var request_size:Int;
	public var minor_version:Int;
	public var headers:Dynamic;
}

@:hlNative("picohttpparser")
class PicoHttpParser {
	@:hlNative("?picohttpparser", "parse_request")
	public static function parse_request(buffer:hl.Bytes, max_header:Int):ParsedRequest {return null;}

	@:hlNative("?picohttpparser", "free_pico_data")
	public static function free_pico_data(parsed_data:ParsedRequest):Void {}
}
