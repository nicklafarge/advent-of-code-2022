using DataFrames

f = open("src/day6/contents.txt", "r")
actual = readline(f)
sample1 = "mjqjpqmgbljsphdztnvjfqwrcgsmlb"
sample2 = "bvwbjplbgvbhsrlpgdmjqwftvncz"
sample3 = "nppdvjthqldpwncqszvftbrmjlhg"
sample4 = "nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg"
sample5 = "zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw"

# you need to add a subroutine to the device that detects a start-of-packet marker in the datastream
#  the start of a packet is indicated by a sequence of four characters that are all different.
function main(datastream::String, id::String)
    characters = collect(datastream)
    function find_n(n::Int)
        ix = 0
        for i=n:length(characters)
            if length(intersect(characters[i-n+1:i])) == n
                ix = i
                break
            end
        end
        return ix
    end
    id_ix1=find_n(4)
    id_ix2=find_n(14)
    println("Pt. 1 ($(id)): $(id_ix1)")
    println("Pt. 2 ($(id)): $(id_ix2)")
    return id_ix
end

id_ix = main(sample1, "1")
main(sample2, "2")
main(sample3, "3")
main(sample4, "4")
main(sample5, "5")

main(actual, "Actual")
println("------------------------------------------------")