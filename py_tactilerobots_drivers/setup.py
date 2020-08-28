from setuptools import setup

package_name = 'py_tactilerobots_drivers'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='nazeh',
    maintainer_email='nazeh@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
        'listener = py_tactilerobots_drivers.uart_vel_driver_node:main'
        ],
    },
)
