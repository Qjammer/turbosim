from conans import ConanFile, CMake, tools


class HelloConan(ConanFile):
	name = "hello"
	version = "0.1"
	settings = "os", "compiler", "build_type", "arch"
	options = {
		"shared": [True, False]
	}
	default_options = {
		"shared": False,
		"boost:shared": True,
		"boost:without_math": True,
		"boost:without_wave": True,
		"boost:without_container": True,
		"boost:without_contract": True,
		"boost:without_exception": True,
		"boost:without_graph": True,
		"boost:without_iostreams": True,
		"boost:without_locale": True, # Change me
		"boost:without_log": True,
		"boost:without_program_options": True,
		"boost:without_random": True,
		"boost:without_regex": True,
		"boost:without_mpi": True,
		"boost:without_serialization": True,
		"boost:without_coroutine": True,
		"boost:without_fiber": True,
		"boost:without_context": True,
		"boost:without_timer": True,
		"boost:without_thread": True,
		"boost:without_chrono": True,
		"boost:without_date_time": True,
		"boost:without_atomic": True,
		"boost:without_filesystem": True, # Change me
		"boost:without_system": True,
		"boost:without_graph_parallel": True,
		"boost:without_python": True,
		"boost:without_stacktrace": True,
		"boost:without_test": True,
		"boost:without_type_erasure": True,
	}
	generators = "cmake"
	exports_sources = "src*"

	source_folder = "src"
	install_folder = "install"
	build_folder = "build"
	package_folder = "package"

	requires = ["boost/1.72.0"]
	
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
		cmake.install()
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
