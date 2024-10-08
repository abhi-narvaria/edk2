/** @file
  CxlDxe driver utility file
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "CxlDxe.h"

inline UINT64 min2(UINT64 a, UINT64 b)
{
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

inline size_t min2size(size_t a, size_t b)
{
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

inline UINT64 min3(UINT64 a, UINT64 b, UINT64 c)
{
  if (a <= b && a <= c) {
    return a;
  } else if (b <= a && b <= c) {
    return b;
  } else {
    return c;
  }
}

UINT64 field_get(UINT64 reg, UINT32 p1, UINT32 p2)
{
  UINT32 X = p1 - p2 + 1;        //Num of bits
  reg = reg >> p2;               //Right shift to make P2 position as 0 bit position
  UINT64 mask = (1 << X) - 1;    //Crate mask
  UINT64 lastXbits = reg & mask;
  return lastXbits;
}

void strCpy(CHAR16 *st1, char *st2)
{
  if (st2[0] == '\0') {
    return;
  }

  int i = 0;
  for (i = 0; st2[i] != '\0'; i++) {
    st1[i] = st2[i];
  }
  st1[i] = '\0';
}

EFI_STATUS pci_uefi_read_config_word(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, UINT32 *val) {
  EFI_STATUS  Status;
  UINT32 Offset = start;

  Status = Private->PciIo->Pci.Read(
             Private->PciIo,
             EfiPciIoWidthUint32,
             Offset,
             1,
             val
             );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[%a]: Failed to read PCI IO for Ext. capability\n", __func__));
  }
  return Status;
}

EFI_STATUS pci_uefi_mem_read_32(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, UINT32 *val) {

  EFI_STATUS  Status;
  UINT32 BarIndex = Private->map.bar;
  UINT32 v32 = 0;

  Status = Private->PciIo->Mem.Read(
             Private->PciIo,
             EfiPciIoWidthUint32,
             BarIndex,
             start,
             1,
             &v32
             );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[%a]: Failed to read PCI Mem\n", __func__));
    return Status;
  }

  *val = v32;
  return Status;
}

EFI_STATUS pci_uefi_mem_read_64(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, UINT64 *val) {

  EFI_STATUS Status;
  UINT32 BarIndex = Private->map.bar;
  UINT64 v64 = 0;

  Status = Private->PciIo->Mem.Read(
             Private->PciIo,
             EfiPciIoWidthUint64,
             BarIndex,
             start,
             1,
             &v64
             );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[%a]: Failed to read PCI Mem\n", __func__));
    return Status;
  }

  *val = v64;
  return Status;
}

EFI_STATUS pci_uefi_mem_read_n(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, CHAR8 Buffer[], UINT32 Size) {

  EFI_STATUS  Status;
  UINT32 BarIndex = Private->map.bar;
  UINT32 offset = start;

  for (int i = 0; i < Size; i++) {
    Status = Private->PciIo->Mem.Read(
               Private->PciIo,
               EfiPciIoWidthUint8,
               BarIndex,
               offset,
               1,
               &Buffer[i]
               );

    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "[%a]: Read err in Buffer[%d] \n", __func__, i));
      break;
    }
    offset += 1;
  }
  return Status;
}

EFI_STATUS pci_uefi_mem_write_32(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, UINT32 *val) {

  EFI_STATUS  Status;
  UINT32 BarIndex = Private->map.bar;
  UINT32 v32 = *val;

  Status = Private->PciIo->Mem.Write(
             Private->PciIo,
             EfiPciIoWidthUint32,
             BarIndex,
             start,
             1,
             &v32
             );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[%a]: Failed to write PCI Mem\n", __func__));
  }
  return Status;
}

EFI_STATUS pci_uefi_mem_write_64(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, UINT64 *val) {

  EFI_STATUS  Status;
  UINT32 BarIndex = Private->map.bar;
  UINT64 v64 = *val;

  Status = Private->PciIo->Mem.Write(
             Private->PciIo,
             EfiPciIoWidthUint64,
             BarIndex,
             start,
             1,
             &v64
             );

  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "[%a]: Failed to write PCI Mem\n", __func__));
  }
  return Status;
}

EFI_STATUS pci_uefi_mem_write_n(CXL_CONTROLLER_PRIVATE_DATA *Private, UINT32 start, CHAR8 Buffer[], UINT32 Size) {

  EFI_STATUS Status;
  UINT32 BarIndex = Private->map.bar;
  UINT32 offset = start;

  for (int i = 0; i < Size; i++) {
    Status = Private->PciIo->Mem.Write(
               Private->PciIo,
               EfiPciIoWidthUint8,
               BarIndex,
               offset,
               1,
               &Buffer[i]
               );

    if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "[%a]: Read err in Buffer[%d] \n", __func__, i));
        break;
    }
    offset += 1;
  }
  return Status;
}
