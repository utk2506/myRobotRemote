BASE_GIT_REPOSITORY=git@github.com:FellowRoboticists

all_libs: lib/AFMotor lib/IRremote lib/LEDBlinker lib/RobotMotor lib/pspc_support

# Set up the libraries we need
lib/AFMotor:
	cd lib; \
		git clone $(BASE_GIT_REPOSITORY)/Adafruit-Motor-Shield-library.git AFMotor; \
		cd AFMotor; \
		git fetch origin robot_kit:robot_kit; \
		git checkout robot_kit

lib/IRremote:
	cd lib; git clone $(BASE_GIT_REPOSITORY)/IRremote.git

lib/LEDBlinker:
	cd lib; git clone $(BASE_GIT_REPOSITORY)/LEDBlinker.git

lib/RobotMotor:
	cd lib; git clone $(BASE_GIT_REPOSITORY)/RobotMotor.git

lib/pspc_support:
	cd lib; git clone $(BASE_GIT_REPOSITORY)/pspc_support.git
