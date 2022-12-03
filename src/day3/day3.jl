using DataFrames

# f = open("src/day3/sample_contents.txt", "r")
f = open("src/day3/contents.txt", "r")


struct Item
    id::Char
    priority::Int64
end

function createItem(id::Char) 
    unicode_offset = isuppercase(id) ? 38 : 96
    return Item(id, Int(codepoint(id))-unicode_offset)
end

df = DataFrame(BagNumber=Int64[], Group=Int64[], CompartmentNumber=Int64[], ItemList=Array{Item}[])
group_counter = 1
for (i, line) in enumerate(readlines(f))
    global group_counter
    compartment_index = Int64(length(line) / 2)
    push!(df,  [i, group_counter, 1, [createItem(id) for id in line[1:compartment_index]]])
    push!(df,  [i, group_counter, 2, [createItem(id) for id in line[compartment_index+1:end]]])
    if (i % 3) == 0
        group_counter += 1
    end
end

n_bags = Int64(nrow(df)/2)
priority_sum = 0
for bag_num=1:n_bags
    global priority_sum
    bag = filter(row -> row.BagNumber == bag_num, df)

    in_both = intersect(union(bag[1,:].ItemList), union(bag[2,:].ItemList))
    for error in in_both
        priority_sum += error.priority
    end


end
println("Priority Sum: $(priority_sum)")

badge_priority_sum = 0
for group_num = 1: maximum(df.Group)
    global badge_priority_sum
    group = filter(row -> row.Group == group_num, df)
    all_items = []
    for i=union(group.BagNumber)
        bag = filter(row-> row.BagNumber==i, group).ItemList
        push!(all_items, union([bag[1]; bag[2]]))
    end
    badge = intersect(all_items[1], all_items[2], all_items[3])[1]
    badge_priority_sum  += badge.priority
end

println("Badge Sum: $(badge_priority_sum)")