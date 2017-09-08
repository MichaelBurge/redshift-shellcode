{-# LANGUAGE OverloadedStrings, ViewPatterns #-}

module Main where

import Data.Elf
import System.Environment
import Data.ByteString as BS hiding (map)
import Data.ByteString.Builder as BSB
import Data.ByteString.Builder.Prim as BSB (primFixed)
import qualified Data.ByteString.Lazy as BSL
import Data.List
import Data.Semigroup
import Data.Word
import qualified Data.Text.Lazy as TL
import qualified Data.Text.Lazy.Encoding as TL
import Prelude hiding (readFile)

main :: IO ()
main = do
  [filename] <- getArgs
  bs <- readFile filename
  let elf = parseElf bs
  dumpElf elf

dumpElf :: Elf -> IO ()
dumpElf elf = do
  let ss = elfSections elf
      sectionsByName = map (\s -> (elfSectionName s, s)) ss
  case lookup ".text" sectionsByName of
    Nothing -> error "No .text section found"
    Just s -> do
      -- Output actual shellcode
      BSL.putStr "shellcode = b'"
      dumpPythonBytes $ elfSectionData s
      BSL.putStr "'\n"
      -- Output offset to the "main" function
      let offset = getOffset elf
      BSL.putStr "offset = "
      BSL.putStr $ TL.encodeUtf8 $ TL.pack $ show offset
      BSL.putStr "\n"
      
  return ()

getOffset :: Elf -> Word64
getOffset elf =
  let stes = Prelude.concat $ parseSymbolTables elf
      textStes = [ x | x@EST { steEnclosingSection = Just (ElfSection { elfSectionName = ".text" }) } <- stes ]
      mainStes = [ x | x@EST { steName = (_, Just "custom_main")} <- textStes ]
      mainOs = Prelude.map steValue mainStes
  in case mainOs of
    [x] -> x
    [] -> error "No main functions found"
    _ -> error $ "Multiple main functions found: " ++ show mainStes
  
dumpPythonBytes :: ByteString -> IO ()
dumpPythonBytes bs = case BS.uncons bs of
  Nothing -> return ()
  Just (b, bs') -> do
    putByte b
    dumpPythonBytes bs'
  where
    putByte b = do
      BSL.putStr $ BSB.toLazyByteString $ "\\x" <> word8HexFixed b
