## Quick start

Compile wallet core with `${PROJECT_ROOT}/bootstrap.sh`
Use `./prepare.sh`
Compile the cli with `cd cmd && go build -o ../tw_dev_console && cd -`
Start `./tw_dev_console`

If there is any compilation error on MacOS you can try the following command:

`go get -u golang.org/x/sys` 

source: https://stackoverflow.com/questions/71507321/go-1-18-build-error-on-mac-unix-syscall-darwin-1-13-go253-golinkname-mus