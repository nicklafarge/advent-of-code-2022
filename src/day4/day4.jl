using DataFrames

function main(;sample=true)
    title_label = sample ? "Sample" : "Actual"
    println("---------------- $(title_label) Content ----------------")
    filename = sample ? "sample_contents" : "contents"
    f = open("src/day4/$(filename).txt", "r")

    counter1 = 0
    counter2 = 0
    elves = []
    for (i, line) in enumerate(readlines(f))
        tasks = split(line, ',')
        efl1_range = split(tasks[1], '-')
        efl2_range = split(tasks[2], '-')
        elf1 = Array(parse(Int64, efl1_range[1]):parse(Int64, efl1_range[2]))
        elf2 = Array(parse(Int64, efl2_range[1]):parse(Int64, efl2_range[2]))
        interelf = intersect(elf1, elf2)

        if length(interelf) == length(elf1) || length(interelf) == length(elf2)
            counter1 += 1
        end
        if length(interelf) > 0
            counter2 += 1
        end
        push!(elves, (i, elf1, elf2, interelf))
    end

    println("Part (1): $(counter1)")
    println("Part (2): $(counter2)")

    return elves, counter1, counter2
end

elves, counter1, counter2 = main(sample=true)

main(sample=false)
println("------------------------------------------------")