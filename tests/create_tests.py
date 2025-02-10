import os

# Ensure the test_files directory exists
os.makedirs('test_files', exist_ok=True)

# Create basic.txt
with open('test_files/basic.txt', 'w') as f:
    f.write('hello world\n')
    f.write('this is a test\n')

# Create large_file.txt (reduced to 1000 lines for practicality)
with open('test_files/large_file.txt', 'w') as f:
    for i in range(1000):
        f.write(f'line {i}\n')

# Create special.txt
with open('test_files/special.txt', 'w') as f:
    f.write('line with $pecial ch@r@cters\n')

# Create long_line.txt
with open('test_files/long_line.txt', 'w') as f:
    f.write('a' * 10000 + '\n')

# Create binary.bin
with open('test_files/binary.bin', 'wb') as f:
    f.write(bytes([0x00, 0x01, 0x02, 0x03]))

print("All test files have been created in the 'test_files' directory.")
