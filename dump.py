# process result of dump
#
#
# generate dump file
# dumpe2fs dev > dump
#
# <exmaple>
# sudo dumpe2fs /dev/nvme0n1 > dump 
# 

free_block_list = []
block_list = []
sector_size = 11

with open('dump', mode='r') as f:
    while True:
        line = f.readline()
        if not line : break
        if 'Free blocks' in line:
            free_block = line.split(': ')
            free_block = free_block[1].replace('\n', '')
            free_block_list.append(free_block)
    
    # 첫번째 free block은 몇개를 쓸 수 있는 지 count한 정보이기 때문에 제거
    free_block_list = free_block_list[1:]

    for i in free_block_list:
        block = i.split('-')

        if '' in block:
            continue

        block[0] = block[0] + '\0' * (11-len(block[0])) 
        block[1] = block[1] + '\0' * (11-len(block[1]))

        block_list.append(block[0])
        block_list.append(block[1])

    print(''.join(block_list))

with open('result', mode='w') as f:
    f.write(''.join(block_list))

