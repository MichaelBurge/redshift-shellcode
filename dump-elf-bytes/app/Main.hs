{-# LANGUAGE OverloadedStrings #-}

module Main where

import Data.Elf
import System.Environment
import Data.ByteString as BS hiding (map)
import Data.ByteString.Builder as BSB
import Data.ByteString.Builder.Prim as BSB (primFixed)
import qualified Data.ByteString.Lazy as BSL
import Data.List
import Data.Semigroup
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
      BSL.putStr "shellcode = b'"
      dumpPythonBytes $ elfSectionData s
      BSL.putStr "'"
  return ()
  
dumpPythonBytes :: ByteString -> IO ()
dumpPythonBytes bs = case BS.uncons bs of
  Nothing -> return ()
  Just (b, bs') -> do
    putByte b
    dumpPythonBytes bs'
  where
    putByte b = do
      BSL.putStr $ BSB.toLazyByteString $ "\\x" <> word8HexFixed b
