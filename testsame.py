# 打开文件并读取每一行数据
with open('build/testdir/1.out', 'r') as file1, open('build/complex/testcase1/1.ans', 'r') as file2:
    # 读取文件内容作为列表
    file1_lines = file1.readlines()
    file2_lines = file2.readlines()

# 比较两份文件，找出差异
with open('diff.txt', 'w') as file_out:
    for idx, line in enumerate(file1_lines):
        if line != file2_lines[idx]:
            file_out.write(f'Difference found at line {idx+1}:\n')
            file_out.write(f'File1: {line.strip()}\n')
            file_out.write(f'File2: {file2_lines[idx].strip()}\n\n')
