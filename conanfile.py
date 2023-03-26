from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMake


# helper function for deploy()
def copy(src, dst):
    if os.path.islink(src):
        if os.path.lexists(dst):
            os.unlink(dst)
        linkto = os.readlink(src)
        os.symlink(linkto, dst)
    else:
        shutil.copy(src, dst)

class Project(ConanFile):
    name = "ieee802154-sniffer"
    description = "Sniffer for IEEE 802.15.4 packets"
    url = "https://github.com/Jochen0x90h/ieee802154-sniffer"
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "platform": [None, "ANY"]}
    default_options = {
        "platform": None}
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "conanfile.py", "CMakeLists.txt", "src/*"
    requires = [
        "coco-ieee802154/pow10",
        "coco-zigbee/main",
        "coco-pcap/main",
    ]


    # check if we are cross compiling
    def cross(self):
        if hasattr(self, "settings_build"):
           return self.settings.os != self.settings_build.os
        return False

    def build_requirements(self):
        self.tool_requires("coco-toolchain/pow10", options={"platform": self.options.platform})

    def configure(self):
        # pass platform option to dependencies
        self.options["coco/*"].platform = self.options.platform
        self.options["coco-buffer/*"].platform = self.options.platform
        self.options["coco-usb/*"].platform = self.options.platform
        self.options["coco-ieee802154/*"].platform = self.options.platform
        self.options["coco-zigbee/*"].platform = self.options.platform
        self.options["coco-file/*"].platform = self.options.platform
        self.options["coco-pcap/*"].platform = self.options.platform

    keep_imports = True
    def imports(self):
        # copy dependent libraries into the build folder
        copy(self, "*", src="@bindirs", dst="bin")
        copy(self, "*", src="@libdirs", dst="lib")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        # run unit tests if CONAN_RUN_TESTS environment variable is set to 1
        #if os.getenv("CONAN_RUN_TESTS") == "1" and not self.cross():
        #    cmake.test()

    def package(self):
        # install from build directory into package directory
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        pass
