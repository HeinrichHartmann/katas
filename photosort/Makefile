build: build-native

build-native:
	nix build

build-windows:
	nix build .#windows

clean:
	rm -rf result

run:
	./result/bin/photosort

run-windows:
	# Wine for OSX is not available on nix
	# Use `brew install --cask wine-stable` to install
	wine ./result/bin/photosort.exe
	