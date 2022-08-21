# 1. Two_Sum

最简单的循环：

```c
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int * result = (int*)malloc(sizeof(int)*2);
    int i = 0,j = 0;
    for (i = 0; i<numsSize; i++)
    {
        for(j = i+1; j<numsSize; j++)
        {
            if(nums[i]+nums[j]==target)
            {
                *returnSize = 2;
                result[0] = i;
                result[1] = j;
                return result;
            }
        }
    }
    *returnSize = 0;
    return result;
}
```

哈希表：

```c
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int * result = (int*)malloc(sizeof(int)*2);
    if (numsSize == 2)
    {
        result[0] = 0;
        result[1] = 1;
        *returnSize = 2;
        return result;
    }
    int i = 0;
    int max = nums[0],min = nums[0];
    for (i = 0; i<numsSize; i++)
    {
        if( max<=nums[i] ) max = nums[i];
        if( min>=nums[i] ) min = nums[i];
    }
    int length = max-min+1;
    int * HashMap = (int *)calloc(sizeof(int),length+1);//为了用calloc()一次完成初始化，所以后续存储nums下标时，需要+1，返回结果时减回去。
    for (i = 0; i<numsSize; i++)
    {
        if(target-nums[i]-min+1>=length+1 || target-nums[i]-min+1 < 1)
        {
            HashMap[ nums[i]-min+1 ] = i+1;
            continue;
        }
        if( HashMap[ target-nums[i]-min+1 ]  )
        {
            result[0] = i;
            result[1] = HashMap[ target-nums[i]-min+1 ]-1;
            free(HashMap);
            *returnSize = 2;
            return result;
        }
        HashMap[ nums[i]-min+1 ] = i+1;
    }
    free(HashMap);
    *returnSize = 0;
    return result;
}
```

