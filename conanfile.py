from conans import ConanFile, CMake, tools


class HelloConan(ConanFile):
	name = "hello"
	version = "0.1"
	settings = "os", "compiler", "build_type", "arch"
	options = {
		"shared": [True, False]
	}
	default_options = {
		"shared": False
	}
	generators = "cmake"
	exports_sources = "src*"

	source_folder = "src"
	install_folder = "install"
	build_folder = "build"
	package_folder = "package"


	requires = ["eigen/3.3.7"]
	
	def configure(self):
		#self.options['icu'].with_dyload = False
		pass


	def source(self):
		pass


	def build(self):
		cmake = self.configure_cmake()
		cmake.build()

		# Explicit way:
		# self.run('cmake %s/hello %s'
		#          % (self.source_folder, cmake.command_line))
		# self.run("cmake --build . %s" % cmake.build_config)

	def package(self):
		cmake = self.configure_cmake()
		#cmake.install()
		self.copy("*", src="bin")
		self.copy("*.h", src="src", dst="include")
		#self.copy("*.a", dst="lib", keep_path=False)
		#self.copy("*", src="package/locale", dst="locale")

	def package_info(self):
		pass
		#self.cpp_info.libs = ["hello"]


	def configure_cmake(self):
		cmake = CMake(self)
		cmake.configure(source_folder="src")
		return cmake
