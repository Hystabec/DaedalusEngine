import subprocess

try:
    import pkg_resources
except ImportError:
    install(setuptools)
    import pkg_resources

def install(package):
    print(f"Installing {package} module...")
    subprocess.check_call(['python3', '-m', 'pip', 'install', package])

def validatePackage(package):
    required = { package }
    installed = { pkg.key for pkg in pkg_resources.working_set }
    missing = required - installed

    if missing:
        install(package)

def validatePackages():
    validatePackage('requests')
    validatePackage('fake-useragent')