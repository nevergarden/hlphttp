package picohttp;

typedef ParsedRequest = {
	var method:String;
	var path:String;
	var request_size:Int;
	var minor_verison:Int;
	var header_count:Int;
	var headers:Map<String, String>;
}

abstract class NativeParsedRequest {
	public var method_len:Int;
	public var method:hl.Bytes;
	public var path_len:Int;
	public var path:hl.Bytes;
	public var request_size:Int;
	public var minor_version:Int;
	public var header_count:Int;
	public var headers:hl.Bytes;
}

@:hlNative("picohttpparser")
class PicoHttpParser {
	public static function parseRequest(request:String, ?max_headers:Int = 100):ParsedRequest {
		hl.Gc.blocking(true);
		var headers:Map<String, String> = new Map<String, String>();
		var buffer = hl.Bytes.fromBytes(haxe.io.Bytes.ofString(request));
		var data = parse_request(buffer, max_headers);

		for(i in 0...data.header_count) {
			var name_len = data.headers.getI32(i*16+0);
			var name_pos = data.headers.getI32(i*16+4);
			var value_len = data.headers.getI32(i*16+8);
			var value_pos = data.headers.getI32(i*16+12);

			headers.set(buffer.sub(name_pos, name_len).toBytes(name_len).toString(),
				buffer.sub(value_pos, value_len).toBytes(value_len).toString());
		}

		var ret:ParsedRequest = {
			method: data.method.toBytes(data.method_len).toString(),
			path: data.path.toBytes(data.path_len).toString(),
			request_size: data.request_size,
			minor_verison: data.minor_version,
			header_count: data.header_count,
			headers: headers
		};
		hl.Gc.blocking(false);
		free_pico_data(data);
		return ret;
	}

	@:hlNative("?picohttpparser", "parse_request")
	static function parse_request(buffer:hl.Bytes, max_header:Int):NativeParsedRequest {
		return null;
	}

	@:hlNative("?picohttpparser", "free_pico_data")
	static function free_pico_data(parsed_data:NativeParsedRequest):Void {}
}
