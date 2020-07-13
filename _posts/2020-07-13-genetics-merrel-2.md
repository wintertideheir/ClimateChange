---
layout: newdefault
title:  Lessons from Introduction to Genetics, by Merrel, Chapter 2
author: wintertideheir
tag:    genetics
---

# Introduction to Genetics, Chapter 2

Chapter 2 of _Introduction to Genetics_ is where Merrel begins talking about Mendel's laws.

## Diligence in Science

Merrel writes about how successful science is carried out in the introduction to this chapter.
It may appear to have no significance for a simulation, but the analytic tools I want to provide are clearly related to scientific inquiry.
It is better for the developer to only have to write simple tools.
It is better for the user to have complex and flexible tools.
What kind of tools might a user need from ClimateChange?

Well, we can take note from the past.
Merrel writes about previous experiments into genetics performed by Kolreuter, Gortner, and Darwin.
These experiments failed in part because their data was too complex to draw conclusions.
That means ClimateChange's tools ought to be capable of sorting and managing data.
Most of the onus still lies on the user, however.

The troubling component of Mendel's success is the meticulous records he had to keep.
Mendel chose traits that were distinct, obvious, and simple in combination.
That means for a Mendelian experiment to be viable in ClimateChange, phenotypes have to be distinct, obvious, and simple.
This is the beginning of the hidden complexity I wrote about in [my post for the first chapter.]({% post_url 2020-07-10-genetics-merrel-1 %})
ClimateChange needs a precise way to describe phenotypes across individuals without resorting to an exact speciation framework.

## The Law of Segregation and Independent Assortment

Mendel hypothesized the existence of independent units of heredity, which we call genes today.
The Law of segregation states that we have two copies of each gene, of which we pass on only one.
The best illustration is the reproduction of two identical hybrids.
The filial generation will have a 1:2:1 genotype ratio of homogeneous individuals for one variant, hybrids, and homogeneous individuals for the other variant.
If one variant is preferentially expressed over the other, the phenotypic ratio will be 3:1.

Although this was difficult for Mendel to discover, we can implement this rather easily in ClimateChange.
Our model should be intrinsically diploid, and functions on genes should include random chance.

When Mendel began studying multiple traits, he recognized that genes segregate independently.
In other words, segregation only happens within the genetic, diploid pair.
This is the law of independent assortment.
If genes are represented intrinsically as a tuple-like structure, than this law should be easy to follow.
