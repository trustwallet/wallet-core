protoc -I=. --go_out=ethereum --go_opt=paths=source_relative Ethereum.proto
protoc -I=. --go_out=common --go_opt=paths=source_relative Common.proto
protoc -I=. --go_out=bitcoin --go_opt=paths=source_relative Bitcoin.proto