.PHONY:	ffi_gen	proto

ffi_gen:
	@echo "* ffi generation *"
	@dart run ffigen

proto:
	@echo "* protobuf generation *"
	@flutter pub global activate protoc_plugin
	@protoc  -I=./gen/proto --dart_out=./lib/src/gen/proto ./gen/proto/**.proto