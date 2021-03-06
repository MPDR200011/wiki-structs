test:
	cd build/ && \
		meson test --print-errorlogs

build:
	cd build/ && \
		meson compile && \
		cp compile_commands.json -t ../

setup:
	meson setup build

clean:
	rm -rf build/
	rm compile_commands.txt
