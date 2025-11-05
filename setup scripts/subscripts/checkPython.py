import subprocess
import sys

def install(package):
    print(f"Installing {package} module...")
    subprocess.check_call(['python3', '-m', 'pip', 'install', package])

def validatePackage(package):
    try: 
       mpkg = __import__(str(package))
    except:
        install(package)

def validatePackages():
    validatePackage('requests')
    validatePackage('fake_useragent')