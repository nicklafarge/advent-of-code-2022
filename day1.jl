using DataFrames

# Open the input file
f = open("input/day1_input.txt", "r")
# f = open("input/day1_sample_input.txt", "r")

# DataFrame to keep track of each elf
df = DataFrame(ElfId=Int64[], Calories=Int64[])

# Total count for each elf (reset after each newline in input file)
calorie_count = 0
elf_num = 1

#  Read lines into dataframe
for line in readlines(f)
    global calorie_count
    
    # Increment the calorie count, or move on to the next elf
    if cmp(line, "")==0
        push!(df, [elf_num, calorie_count])
        calorie_count = 0
        elf_num += 1
    else
        calorie_count += parse(Int64, line)
    end

end

# Add the final elf
if calorie_count > 0
    push!(df, [elf_num, calorie_count])
    calorie_count = 0
end

# Part 1: elf carrying the most calories
elf_by_calories = sort(df, order(:Calories))
top_3 = last(elf_by_calories,3)
println("Top 1 calories: $(top_3[3,:].Calories)")

# Part 2: Sum of all calories from top 3 elves
top3_cal_sum = sum(top_3.Calories)
println("Top 3 sum: $(top3_cal_sum)")