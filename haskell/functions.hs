import System.IO
import GHC.IO
import Control.Parallel

--connections = [(1,2),(2,4),(2,5),(3,2),(4,2),(4,5),(5,3),(5,4)]
connections = [(1,2),(2,3),(3,4),(4,5),(5,6),(2,4)]

matFromPairs :: [(Int, Int)] -> [[Int]]
matFromPairs xs = [ [if (y,x) `elem` xs then 1 else 0 | x <- w ] | y <- h]
	where h = [1..maximum . maximum $ xs]
	      w = h
--
--connect :: [[Int]] -> Int -> Int -> [[Int]]
--connect m y x  = [[ (y==j && x==i) || (m !! j !! i) | i <- [0 .. length (m !! j) - 1]] | j <- [0..length m - 1]]
--
matMult :: [[Int]] -> [[Int]] -> [[Int]]
matMult a b = [ [ sum [ (a !! y !! z) * (b !! z !! x) | z <- d] | x <- w ] | y <- h ]
              where w = [0..(length (b !! 0))-1]
                    h = [0..(length a)-1]
                    d = [0..(length b)-1]

matAdd :: [[Int]] -> [[Int]] -> [[Int]]
matAdd a b =  [ [ (a !! y !! x) + (b !! y !! x) | x <- w ] | y <- h ]
              where w = [0..(length (a !! 0))-1]
                    h = [0..(length a)       -1]

nthPairs :: [[Int]] -> Int -> [[Int]]
nthPairs m 0 = m
nthPairs m i = m `matMult` (nthPairs m (i-1))

upToNthPairs :: [[Int]] -> Int -> [[Int]]
upToNthPairs m i = foldr matAdd m $ map (nthPairs m) [1..i]

main = do
	putStr "connections:           "
	putStrLn . show $ connections
	putStr "connections as matrix: "
	let matPairs = matFromPairs $ connections
	putStrLn . show $ matPairs
	let indirectPairs = upToNthPairs matPairs
	putStrLn "indirect connections:  "
	putStrLn . show $ indirectPairs 1
	putStrLn . show $ indirectPairs 2
	putStrLn . show $ indirectPairs 3
	putStrLn . show $ indirectPairs 4
	putStrLn . show $ indirectPairs 5
