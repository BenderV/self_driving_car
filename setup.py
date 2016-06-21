from setuptools import setup, find_packages
setup(
    name = 'selfdrivutt',
    packages = find_packages(),
    package_data={'ai': ['resources/haar/*.xml']}
)