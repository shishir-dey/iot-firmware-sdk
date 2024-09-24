import json
import glob
import os

# Find the first available config/device_*.json file
config_files = glob.glob('config/device_*.json')
if not config_files:
    raise FileNotFoundError("No config/device_*.json file found")

config_file = config_files[0]

# Read the JSON file
with open(config_file, 'r') as f:
    config_data = json.load(f)

# Helper function to format certificate strings
def format_certificate(cert):
    lines = cert.split('\n')
    formatted_cert = ' \\\n'.join([f'"{line}\\r\\n"' for line in lines if line])
    return formatted_cert

# Create the C header file content
header_content = """
#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#define PROJECT_ID "{}"
#define BROKER "{}"
#define PORT {}
#define DEVICE_ID "{}"\n
#define CA_CERTIFICATE  {}\n
#define DEVICE_CERTIFICATE  {}\n
#define DEVICE_PRIVATE_KEY  {}

#endif // DEVICE_CONFIG_H
""".format(
    config_data['project_id'],
    config_data['broker'],
    config_data['port'],
    config_data['device_id'],
    format_certificate(config_data['authentication']['ca_certificate']),
    format_certificate(config_data['authentication']['device_certificate']),
    format_certificate(config_data['authentication']['device_private_key'])
)

# Write the C header file
header_file_path = 'config/device_config.h'
with open(header_file_path, 'w') as f:
    f.write(header_content)

print(f"Generated {header_file_path} from {config_file}")



